export MAGICK_HOME="$HOME/apps/ImageMagick-6.8.9"
export PATH="$MAGICK_HOME/bin:$PATH"
export DYLD_LIBRARY_PATH="$MAGICK_HOME/lib/"

convert -extract 8x10+0+0 ../images/number_score.png ../images_processed/font_48.png
convert -extract 8x10+8+0 ../images/number_score.png ../images_processed/font_49.png
convert -extract 8x10+16+0 ../images/number_score.png ../images_processed/font_50.png
convert -extract 8x10+24+0 ../images/number_score.png ../images_processed/font_51.png
convert -extract 8x10+32+0 ../images/number_score.png ../images_processed/font_52.png
convert -extract 8x10+40+0 ../images/number_score.png ../images_processed/font_53.png
convert -extract 8x10+48+0 ../images/number_score.png ../images_processed/font_54.png
convert -extract 8x10+56+0 ../images/number_score.png ../images_processed/font_55.png
convert -extract 8x10+64+0 ../images/number_score.png ../images_processed/font_56.png
convert -extract 8x10+72+0 ../images/number_score.png ../images_processed/font_57.png

convert -extract 6x7+0+0 ../images/number_context.png ../images_processed/number_score_48.png
convert -extract 6x7+6+0 ../images/number_context.png ../images_processed/number_score_49.png
convert -extract 6x7+12+0 ../images/number_context.png ../images_processed/number_score_50.png
convert -extract 6x7+18+0 ../images/number_context.png ../images_processed/number_score_51.png
convert -extract 6x7+24+0 ../images/number_context.png ../images_processed/number_score_52.png
convert -extract 6x7+30+0 ../images/number_context.png ../images_processed/number_score_53.png
convert -extract 6x7+36+0 ../images/number_context.png ../images_processed/number_score_54.png
convert -extract 6x7+42+0 ../images/number_context.png ../images_processed/number_score_55.png
convert -extract 6x7+48+0 ../images/number_context.png ../images_processed/number_score_56.png
convert -extract 6x7+54+0 ../images/number_context.png ../images_processed/number_score_57.png
convert -extract 6x7+60+0 ../images/number_context.png ../images_processed/number_score_58.png
