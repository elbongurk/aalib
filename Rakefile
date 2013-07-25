require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :test

desc "Build, install, and run tests"
task :default => [:build, :install, :test]
