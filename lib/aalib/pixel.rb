module AAlib
  class Pixel
    attr_reader :chr, :attr, :color

    def initialize(ascii, attr, color)
      @chr = ascii.chr
      @attr = attr
      @color = color
    end

    def normal?
      self.attr == Attr::NORMAL
    end

    def dim?
      self.attr == Attr::DIM
    end

    def bold?
      self.attr == Attr::BOLD
    end

    def boldfont?
      self.attr == Attr::BOLDFONT
    end
  end
end
