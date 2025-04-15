require 'cucumber/rake/task'

Cucumber::Rake::Task.new(:cucumber) do |t|
  profile = ENV['CUCUMBER_PROFILE'] || 'default'
end

desc 'Run Cucumber tests'
task :test => :cucumber

desc 'Default task runs tests'
task :default => :test