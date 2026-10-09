#!/usr/bin/env bash

# Aufruf:  ./benchmark.sh
# Vorher:  chmod +x benchmark.sh

APP="../build/apps/EntitysInBox"
CSV="benchmark.csv"   
HEADER="n,output\n"      # Kopfzeile; passe sie an das an, was dein Programm ausgibt
SIZES=(10 20 50 100 200 500 1000)   # Problemgroessen

if [[ ! -x "$APP" ]]; then
    echo "Fehler: '$APP' nicht gefunden oder nicht ausfuehrbar." >&2
    exit 1
fi

: > "$CSV"

echo "$HEADER" >> "$CSV"

for n in "${SIZES[@]}"; do
    echo "Starte: $APP $n" >&2          # Fortschritt im Terminal (stderr)

    # Ausgabe des Programms in einer Variablen einfangen
    output=$("$APP" "$n")

    # Falls das Programm fehlschlaegt: Abbruch mit Meldung
    if [[ $? -ne 0 ]]; then
        echo "Fehler: '$APP $n' ist fehlgeschlagen." >&2
        exit 1
    fi

    # Zeile "n,ausgabe" an die CSV anhaengen
    echo "$n,$output" >> "$CSV"
done

echo "Fertig. Ergebnisse in $CSV" >&2
