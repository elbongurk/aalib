require "test_helper"

class RenderTest <  Test::Unit::TestCase
  def test_attr_can_be_used
    width = 150
    height = 150

    a = AAlib::Context.new(width, height)

    height.times do |y|
      width.times do |x|
        a.putpixel(x, y, 127 * (x.to_f/width) + 127 * (y.to_f/height))
      end
    end

    assert_nothing_raised do
      a.render.each do |pixel|
        case pixel.attr
        when AAlib::Attr::NORMAL
        when AAlib::Attr::DIM
        when AAlib::Attr::BOLD
        when AAlib::Attr::BOLDFONT
        end
      end
    end
  end
end








