require "test/unit"
require "aalib"

class AttrsTest <  MiniTest::Unit::TestCase
  def test_attrs_is_the_correct_size_once_image_is_filled
    width = 150
    height = 150

    a = AAlib.new(width, height)

    height.times do |y|
      width.times do |x|
        a.putpixel(x, y, Random.rand(256))
      end
    end

    a.render

    assert_equal a.scrwidth * a.scrheight, a.attrs.unpack("C*").size
  end
end
