#!/usr/bin/env ruby
require 'open3'

num_files = (1..7).to_a.reverse
type = (1..4).to_a

class TestRun < Struct.new(:klass, :result)
  def self.from_run(klass, list_path)
    # this lets me the script without an argument
    if list_path.nil?
      list_path = "test_files/given/pa5/correctfiles.txt"
    end
    cmd = ["./PA8", list_path, klass.to_s]
    stdin, stdout, stderr, wait_thr = Open3.popen3(*cmd)
    output = stdout.read
    # status code wasn't zero, fail
    if wait_thr.value != 0
      raise <<eos
    COMMAND FAILED "(#{wait_thr.value}) #{cmd.join(" ")}"
      with msg "#{stderr.read}"
eos
    end
    stdin.close
    stdout.close
    stderr.close
    self.new(klass, output)
  end

  def to_s
    "./PA6 LIST #{klass}:\n#{result}"
  end
end



output = (1..4).to_a.map do |klass|
  TestRun.from_run(klass, ARGV[0])
end

puts output.join("\n\n")
