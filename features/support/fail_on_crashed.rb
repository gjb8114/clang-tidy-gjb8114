After do
    if defined?(@stderr) && @stderr.include?('Stack dump:')
        @crashed = true
        fail "Crash dump detected."
    end
end