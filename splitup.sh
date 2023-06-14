#!/bin/bash

./getpages elliptic_curve_dev.log
./pdfsplit
chmod 775 genpdf.sh
echo "splitting out pages"
./genpdf.sh
dir="rosing2_elliptic_curve_dev_`date +"%Y-%m-%d-%H-%M"`"
mkdir $dir
echo "pdf's to $dir"
mv *rosing2*.pdf $dir
mkdir $dir/rosing2_elliptic_curve_dev_tex
cp *.png $dir/rosing2_elliptic_curve_dev_tex
cp *.tex $dir/rosing2_elliptic_curve_dev_tex
cp *.bib $dir/rosing2_elliptic_curve_dev_tex
#cd $dir/tex
#for f in *
#do
#    ext=${f##*.}
#    name=${f%.*}
#    mv $f "${name}_rosing2_elliptic_curve_dev.${ext}"
#done
