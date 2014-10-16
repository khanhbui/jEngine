java AtlasGenerator atlas 1024 1024 4 2 2 ../data/images_processed/jQuiz
cp atlas*.png ../gdx/
cp atlas*.txt ../gdx/
cp atlas*.png ../android/res/raw/
cp atlas*.txt ../android/res/raw/
mv ../android/res/raw/atlas?.txt ../android/res/raw/atlas?_text.txt
rm atlas*.png
rm atlas*.txt
