#!/bin/bash
# ORDER: old directory first, new directory second

dir="rosing2_elliptic_curve_dev_diff_`date +"%Y-%m-%d-%H-%M"`"
mkdir $dir

dir1="$1/rosing2_elliptic_curve_dev_tex"
dir2="$2/rosing2_elliptic_curve_dev_tex"

files1=(ch*.tex)
files2=(pa*.tex)

lench=${#files1[@]}
for ((i=0; i<$lench; i++))
do
    latexdiff -c ld.cfg --graphics-markup=1 "$dir1/${files1[$i]}" "$dir2/${files1[$i]}" > "$dir/${files1[$i]}"
    ./mvdef "$dir/${files1[$i]}"
done

lenpa=${#files2[@]}
for ((i=0; i<$lenpa; i++))
do
    latexdiff  -c ld.cfg  --graphics-markup=1 "$dir1/${files2[$i]}" "$dir2/${files2[$i]}"  > "$dir/${files2[$i]}"
    ./mvdef "$dir/${files2[$i]}"
done

latexdiff  -c ld.cfg --graphics-markup=1 $dir1/appendix.tex $dir2/appendix.tex > $dir/appendix.tex
./mvdef $dir/appendix.tex
latexdiff  -c ld.cfg --graphics-markup=1  $dir1/appendix_b.tex $dir2/appendix_b.tex  > $dir/appendix_b.tex
./mvdef $dir/appendix_b.tex
latexdiff  -c ld.cfg --graphics-markup=1  $dir1/preface.tex $dir2/preface.tex > $dir/preface.tex
./mvdef $dir/preface.tex
latexdiff  -c ld.cfg --graphics-markup=1  $dir1/roadmap.tex $dir2/roadmap.tex > $dir/roadmap.tex
./mvdef $dir/roadmap.tex
# bring in files so xelatex can run
echo "copy basic files"
cp -r fonts $dir
cp manning-new.cls $dir
cp *.png $dir
cp *.jpg $dir
echo "creating pdfs"
cd $dir
for f in *.text
do
    echo "doing $f"
    cat ../diff_preamble.text "$f" ../end.text > "${f%.text}.tex"
    xelatex -synctex=1 -interaction=nonstopmode "${f%.text}.tex" > /dev/null
    rm $f
done
# clean up for upload to Manning
rm *.aux
rm *.cb
rm *.cb2
rm *.idx
rm *.log
rm *.gz
rm *.png
rm *.jpg
rm *.cls
rm -r fonts
cd ..

