Before do
  @test_dir = File.join(Dir.pwd, "tmp_tests")
  FileUtils.mkdir_p(@test_dir)
end

After do
  FileUtils.rm_rf(@test_dir) if File.exist?(@test_dir)
end
