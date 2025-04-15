require 'colorize'

After do |scenario|
  if scenario.failed?
    puts "#{@stdout}".colorize(:blue) if defined?(@stdout)
    if defined?(@stderr)
      @crashed = @stderr.include?('Stack dump:')
      color = @crashed ? :red : :yellow
      puts "#{@stderr}".colorize(color)
    end
  end
end
