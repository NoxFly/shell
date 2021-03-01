binome1="RUSSO"
binome2="THIVOLLE"

name="$binome1-$binome2.tar.gz"
dir="./${PWD##*/}"

cd ..

if [ -f "$name" ]; then
    rm $name
    echo "Ancien tar.gz supprimé"
fi

tar czf "$name" "$dir"

if [ $? -eq 0 ]; then
    echo "Dossier compressé : ../$name"
else
    echo "Erreur lors de la compression"
fi