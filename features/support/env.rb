require 'cucumber'
require 'fileutils'
require 'open3'

def try_paths(*paths)
  paths.find { |path| File.exist?(path) }
end

def locate_clang_tidy
  if RUBY_PLATFORM.downcase.include?("darwin")
    clang_tidy = "/opt/homebrew/opt/llvm/bin/clang-tidy"
    return clang_tidy if try_paths(clang_tidy)
  end
  fail "cannot found clang-tidy, already tried: #{clang_tidy}"
end

def locate_plugin(bin_dir)
  suffix = case RUBY_PLATFORM.downcase
         when /darwin/ then "dylib"
         when /linux/ then "so"
         else "dll"
         end
  name = File.join('lib', "libclang-tidy-gjb8114.#{suffix}")
  if bin_dir
    plugin = try_paths(File.join(bin_dir, name))
    fail "Plugin #{name} not found, tried BIN_DIR: #{bin_dir}" if plugin.nil?
    return plugin
  end

  paths = %w(debug release).map { |build_type| File.join('build', build_type, name) }
  plugin = try_paths(*paths)
  fail "Plugin #{name} not found, tried: #{paths}" if plugin.nil?
  plugin
end

BeforeAll do
  if !ENV['CLANG_TIDY_PATH'].nil?
    fail "CLANG_TIDY_PATH=#{ENV['CLANG_TIDY_PATH']} is not found" unless File.exist?(ENV['CLANG_TIDY_PATH'])
    clang_tidy = ENV['CLANG_TIDY_PATH']
  end
  clang_tidy ||= `which clang-tidy`.strip.empty? ? locate_clang_tidy : "clang-tidy"

  plugin ||= locate_plugin(ENV['BIN_DIR'])

  $clang_tidy, $plugin = clang_tidy, plugin
end
