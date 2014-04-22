# aalib

Ruby wrapper for aalib. Like the awesome [aalib-ruby](http://aalib-ruby.rubyforge.org) but doesn't require you to have aalib installed as a lib.

## Installation

Add this line to your application's Gemfile:

    gem 'aalib', :github => 'elbongurk/aalib'

And then execute:

    $ bundle

## Usage

    width, height = 300, 200
    a = AAlib::Context.new(width, height)

    height.times do |y|
      width.times do |x|
        # Greyscale values go in from 0-255
        a.putpixel(x, y, Random.rand(256))
      end
    end

    # Compute!
    a.render

    # scrheight & scrwidth are 50% size of height & width
    a.scrheight.times do |y|
      a.scrwidth.times do |x|
        # Retrieve ASCII character value
        a.getpixel(x, y)
        # Retrieve aalib attribute value
        a.getattr(x, y)
        # Retrieve greyscale color value
        a.getcolor(x, y)
      end
    end
    
## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
