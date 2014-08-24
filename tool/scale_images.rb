require 'RMagick'

pathIn = "../data/images/"
pathOut = "../data/images_processed/"
Dir.new(pathIn).each { |e|
    file = pathIn + e
    if File.file?(file)
        if File.extname(file) == ".png"
            puts file
            img = Magick::Image::read(file).first
            puts "   Format: #{img.format} | #{img.columns}x#{img.rows} | #{img.depth} bits-per-pixel | #{img.number_colors} colors | #{img.filesize} bytes"
            simg = img.scale(0.5)
            simg.write pathOut + e
            puts "   --> Format: #{simg.format} | #{simg.columns}x#{simg.rows} | #{simg.depth} bits-per-pixel | #{simg.number_colors} colors | #{simg.filesize} bytes"
        end
    end
}
