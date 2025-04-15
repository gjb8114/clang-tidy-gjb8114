require 'fileutils'
require 'open3'
require 'colorize'

假如('有一个源码文件 {string}，内容如下:') do |filename, file_contents|
  @source_file = File.join(@test_dir, filename)
  File.write(@source_file, file_contents)
end

当('对 {string} 运行带有 gjb8114 插件的 clang-tidy 检查 {string} 时') do |filename, rule|
  path = File.join(@test_dir, filename)
  fail "源码文件 '#{filename}' 不存在于 '#{path}'，请检查步骤描述！" unless File.exist?(path)
  command = "#{$clang_tidy} #{path} -checks='-*, #{rule}' -fix -header-filter=.*  -load='#{$plugin}'"
  puts "    >_: #{command}".colorize(:light_black)
  @stdout, @stderr, @status = Open3.capture3(command)
end

那么('应该报告 {string}') do |expected|
  output = @stdout
  unless output.include?(expected)
    fail "预期诊断信息 '#{expected}' 未在输出中找到。"
  end
end

那么('不应该报告 {string}') do |unexpected|
  output = @stdout
  if output.include?(unexpected)
    fail "意外发现诊断信息 '#{unexpected}' 在输出中。"
  end
end

Summary = /(\d+) warning[s]? (and (\d+) error[s]? )?generated\./.freeze
Suppressed = /Suppressed (\d+) warnings/.freeze

那么('总共报告 {int} 个 warning') do |expected_count|
  output = @stderr
  actual_warnings = output.scan(Summary).flatten.first.to_i
  suppressed_warnings = output.scan(Suppressed).flatten.first.to_i
  actual_warnings -= suppressed_warnings
  unless actual_warnings == expected_count
    fail "预期报告 '#{expected_count}' 个 warning，实际报告: '#{actual_warnings}' 个。"
  end
end

那么('不应该报告任何 warning') do
  step '总共报告 0 个 warning'
end