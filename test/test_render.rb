require "test/unit"
require "aalib"

class RenderTest <  Test::Unit::TestCase
  def test_render_returns_correct_size_when_filled
    width = 150
    height = 150

    a = AAlib::Context.new(width, height)

    height.times do |y|
      width.times do |x|
        a.putpixel(x, y, Random.rand(256))
      end
    end

    assert_equal a.scrwidth * a.scrheight, a.render.size
  end
end








