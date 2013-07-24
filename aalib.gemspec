# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'aalib/version'

Gem::Specification.new do |spec|
  spec.name          = "aalib"
  spec.version       = AAlib::VERSION
  spec.authors       = ["Ryan Krug"]
  spec.email         = ["ryan@elbongurk.com"]
  spec.description   = %q{Ruby binding to aalib}
  spec.summary       = %q{Ruby binding to aalib}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions = ['ext/aalib/extconf.rb']

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
end
