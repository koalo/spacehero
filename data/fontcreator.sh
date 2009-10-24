# FONTCREATOR
# Erstellt eine Textur mit einer Schrift

# HIER EINSTELLEN
SIZE=20                        # Breite und Hoehe eines Zeichens
POINT=30                       # Groesse eines Zeichen im Ausgangsbild (hoeher=>schaerfer aber langsamer)
FONT=-*-freesans-bold-r-*-*-*-*-*-*-*-*-*-*
#FONT=Penguin-Attack-Regular    # Schriftart - Achtung: Es muessen alle noetigen Zeichen vorhanden sein (z.B. °)

# HIER NUR EINSTELLEN WENN LUSTIG
row=0
col=0

rm font.png line.png char.png

while [ $row -lt 16 ]
do

while [ $col -lt 16 ]
do
 CHAR=$(echo $(( 16 * $row + $col + 32 )) | awk '{printf "%c", $1}')
 
 err=$(convert -background black -fill white -pointsize $POINT -font $FONT label:$CHAR char.png 2>&1)
 echo -n "$CHAR"

 if [ "$err" != "" ]; then
   convert -geometry $SIZE\x$SIZE xc:black char.png
 else
   convert char.png -bordercolor black -resize $SIZE\x$SIZE\> -size $SIZE\x$SIZE xc:black +swap -gravity center -composite char.png
 fi
 
 if [ -e line.png ]; then
   montage -geometry +0+0 line.png char.png line.png 
 else 
   mv char.png line.png
 fi

 col=`expr $col + 1`    
done

echo 

if [ -e font.png ]; then
  montage -tile 1x -geometry +0+0 font.png line.png font.png
  rm line.png
else 
  mv line.png font.png
fi

row=`expr $row + 1`    
col=0

done

   convert  font.png +matte \
           \( +clone -fill white -colorize 100% \) \
           +swap -compose CopyOpacity -composite \
           font.png

