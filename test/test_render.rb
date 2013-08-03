require "test_helper"

class RenderTest <  Test::Unit::TestCase
  def test_render_allows_getters
    width = 150
    height = 150

    a = AAlib::Context.new(width, height)

    height.times do |y|
      width.times do |x|
        a.putpixel(x, y, Random.rand(256))
      end
    end

    a.render

    assert_nothing_raised do
      a.scrheight.times do |y|
        a.scrwidth.times do |x|
          a.getpixel(x, y)
          a.getattr(x, y)
          a.getcolor(x, y)
        end
      end
    end
  end
end








