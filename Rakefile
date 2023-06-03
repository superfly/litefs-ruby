require "rake/extensiontask"
require 'rubygems/package_task'

Rake::ExtensionTask.new "litefs" do |ext|
  ext.lib_dir = "lib/litefs"
end

Gem::PackageTask.new(Gem::Specification.load('litefs.gemspec')) do |pkg|
  pkg.need_tar = true
  pkg.need_zip = true
end
