# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "litefs"
  s.version = "0.0.1"

  s.require_paths = ["lib", "ext"]
  s.authors = ["Sam Ruby"]
  s.description = "LiteFS module"
  s.email = "rubys@intertwingly.net"
  s.files = %w(litefs.gemspec) + Dir["lib/**/*.rb", "ext/litefs/*.{rb,c}"]
  s.extensions = %w[ext/litefs/extconf.rb]
  s.homepage = "http://github.com/superfly/litefs-ruby"
  s.licenses = ["Apache-2.0"]
  s.summary = "LiteFS module"
end
