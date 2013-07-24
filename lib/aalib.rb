require "aalib/aalib"
require "aalib/version"

class AAlib
  def pretty
    ascii = self.text
    height = self.scrheight
    width = self.scrwidth

    output = String.new

    height.times do |y|
      width.times do |x|
        output << ascii[y * width + x]
      end
      output << "\n"
    end

    puts output
  end
end
