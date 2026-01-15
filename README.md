# SnapSystem

## MODIFICHE SNAP TaskBuilderStudio_Developer_Edition_5.2.0.HF3

### DA AGGIUNGERE A DETTAGLIO SU ORDINI CLIENTE

- **Costo ultimo** che prende valori in mago → Articolo → Valorizzazione
  - Articolo - anno - storage - specificator type → `SELECT * FROM MA_ItemsBalances`
  - Questo per trovare il costo ultimo
  - ✅ Già fatto, solo da controllare.
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:348-367` (metodo `GetLastCostFromBalances()`)

- **Colonna margine** tra valore unitario - costo ultimo → `((Imponibile – costo) * quantità)`
  - ✅ Già fatto, solo da controllare.
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:502-589` (metodo `CalculateMarginAndCommission()`)

### DA MODIFICARE IN DETTAGLIO ORDINE CLIENTE

- Modificare il valore del campo, si fa il calcolo → `Margine * (Provvigione base in Agente/100)`, questo calcolo è solo per la politica provvigionale 01.
  - ✅ Già fatto, solo da controllare.
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:570-576` (Policy 01 logic in `CalculateMarginAndCommission()`)

- **Ricalcoli automatici**:
  - Al cambio Costo fai il ricalcolo
  - Al cambio valore unitario rifai il calcolo
  - Al cambio quantità fai il ricalcolo
  - ✅ Già fatto, solo da controllare.
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:593-665` (handlers `OnItemChanged()`, `OnQuantityChanged()`, `OnUnitValueChanged()`)

### ANAGRAFICA AGENTI

- **O costo ultimo**, o prezzo di listino scontato, da decidere in base al codice 'Politica Provvigionale' in Anagrafica Agenti, **Politica Provvigionale definisce il tipo di calcolo della provvigione**:
  - Codice 01: costo ultimo
  - Codice 02: prezzo di listino scontato
  - ✅ Già fatto, solo da controllare.
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:398-419` (metodo `GetCostByPolicy()`)

### ARTICOLO

- **Listino**: controllare se la data dell'ordine è dentro tra inizio e fine validità, poi bisogna controllare:
  - La quantità dell'ordine sia inferiore alla quantità del listino
  - Il listino sia con il codice definito (campo Listino, valore 10 per il controllo)
  - Nel caso di più righe, prendo quella più recente (TOP 1 ordinata al contrario, ordino per data inizio validità più recente)
  - Per lo sconto controlliamo `Discount1` e `Discount2` dalla tabella `MA_ItemsPriceLists`
  - **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:134-251` (classe `RRItemsPriceListsByItem`)

#### AGGIUNGERE COLONNA 'Costo Minimo' -> Valore creato come base di partenza al posto del costo ultimo.

✅ Già fatto, solo da controllare.
- **Implementazione**: `SnapSystemDocuments/CDItemsPriceListsAddOn.cpp:65-110` (metodo `CalculateMinimumCost()`)

**Logica calcolo Costo Minimo**:

Esempio: Articolo con prezzo nel listino 450

- La colonna 'Costo Minimo' nel listino sarà uguale a: `prezzo nel listino – (prezzo * (sconto / 100))`
- Se ho un altro sconto: `risultato – (risultato * (sconto2/100))`
- Per gli sconti guardare nella tabella `MA_ItemsPriceLists`: `Discount1` e `Discount2` (possono esserci solo Discount1 senza Discount2)

**Esempio concreto**:
- Prezzo listino: 450
- Sconto 10 + 5
- Calcolo: `450 – (450 *(10/100)) = 405`
- `405 – (405 * (5/100)) = 405 – 20,25 = 384,75`

✅ Già fatto, solo da controllare.

#### CALCOLO PROVVIGIONE

**CASO MAGGIORE** (Valore unitario > prezzo listino)

Esempio: Valore unitario = 500, prezzo listino = 450

1. Differenza tra valore unitario e prezzo listino: `500 - 450 = 50`
2. Provvigione da mettere in ordine cliente:
   ```
   prezzo listino – prezzo minimo + (differenza * (40 / 100))
   = (450 - 384,75) + ( 50 * (40/100))
   = 65,25 + 20
   = 85,25
   ```

✅ Già fatto, solo da controllare.
- **Implementazione**: `SnapSystemDocuments/CDSalesOrderAddOn.cpp:557-569` (Policy 02 logic in `CalculateMarginAndCommission()`)

**Nota**: Per il 40 abbiamo aggiunto un campo chiamato 'Percentuale Massima Scostamento' in `MA_SalesPeople` e messo di default 40, in modo tale che se un giorno loro volessero cambiarlo, possono farlo.
- **Implementazione**: `SnapSystemDbl/TSalesPeopleAddOn.cpp:21-38` (field `f_MaxDeviationPerc`, default 40), `SnapSystemDocuments/CDSalesOrderAddOn.cpp:471-493` (metodo `GetMaxDeviationPerc()`)

❌ Ho aggiunto il campo 'Percentuale Massima Scostamento' in Agente però non compare, anche nel database è presente e prendo da la il valore, quindi se si riesce a capirne il motivo per cui non si vede. Ho cercato di mettere il campo sotto a Commissione di base.

**CASO MINORE** (Valore unitario < prezzo listino)

Esempio: Valore unitario = 420, prezzo listino = 450

1. Differenza tra valore unitario e prezzo listino: `420 - 450 = -30`
2. Provvigione da mettere in ordine cliente:
   ```
   prezzo listino – prezzo minimo + (differenza * (40 / 100))
   = 65,25 + (- 30 * 40/100)
   = 65,25 – 12
   = 53,25
   ```

✅ Già fatto, solo da controllare.

### AGGIUNTA CAMPI COSTO ULTIMO E MARGINE

Le colonne **Costo**, **Margine** sono da aggiungere anche su:
- Documenti di trasporto
- Fatture

**Posizione**: Vendite → Documenti Vendita

⚠️ In questo momento se faccio LoadDocument → Sales Order e poi seleziono l'ordine che desidero caricare, i campi aggiunti non si caricano.

❌ Non Fatta

### PROCEDURE DA FARE

La procedura deve:
- Filtrare solo per data E per ordini (da nr a nr)
- Aggiornare i campi nelle fatture:
  - Costo Ultimo
  - Margine
  - Provvigione

Nella procedura si può filtrare o solo per data o per ordini, da nr a nr.
La procedura deve aggiornare i campi Costo Ultimo, Margine e Provvigione del documento Fatture.
**La procedura serve perché spesso loro aggiungo i costi dopo l’emissione dei documenti di vendita, quindi è necessario andare a ricalcolare le provvigioni come sopra descritto.
Questo ricalcolo serve solo per la politica provvigionale 01.**


**Documento**: Fatture

❌ Non Fatta

---

## RIFERIMENTI FILE DI IMPLEMENTAZIONE

### File Principali di Logica

1. **CDSalesOrderAddOn.cpp** (`SnapSystem/SnapSystemDocuments/`)
   - Gestione Ordini Cliente
   - Metodi chiave: `GetLastCostFromBalances()`, `CalculateMarginAndCommission()`, `GetCostByPolicy()`
   - Handlers per ricalcoli automatici

2. **CDItemsPriceListsAddOn.cpp** (`SnapSystem/SnapSystemDocuments/`)
   - Gestione Listini Articoli
   - Metodo chiave: `CalculateMinimumCost()`

3. **TSalesPeopleAddOn.cpp** (`SnapSystem/SnapSystemDbl/`)
   - Definizione campi Anagrafica Agenti
   - Campo `f_MaxDeviationPerc` con default 40

### Script Database (SQL)

1. **01_AlterSalesOrdDetails_AddMarginFields.sql** (`SnapSystem/DatabaseScript/Create/All/`)
   - Aggiunge campi: `LastCost`, `Margin`, `SalesPersonMarginComm` a `MA_SaleOrdDetails`

2. **02_AlterItemsPriceLists.sql** (`SnapSystem/DatabaseScript/Create/All/`)
   - Aggiunge campo: `MinimumCost` a `MA_ItemsPriceLists`

3. **03_AlterSalesPeople.sql** (`SnapSystem/DatabaseScript/Create/All/`)
   - Aggiunge campo: `MaxDeviationPerc` a `MA_SalesPeople` (default 40)

### File di Registrazione

1. **SnapSystemDblInterface.cpp**
   - Registra le classi AddOn per le tabelle database

2. **SnapSystemDocumentsInterface.cpp**
   - Registra i ClientDocument per l'interfaccia utente
