#!/usr/bin/env ruby
require 'open3'

type = (2..7).to_a

class TestRun < Struct.new(:train_path, :test_path, :num_groups, :result)
  def self.from_run(train_path, test_path, num_groups)
    # this lets me the script without an argument
    train_path ||= "test_files/given/perceptron_train/correctfiles.txt"
    test_path ||= "test_files/given/pa5/correctfiles.txt"
    cmd = ["./PA8", train_path, test_path, num_groups.to_s]
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
    self.new(train_path, test_path, num_groups, output)
  end

  def to_s
    "./PA6 #{train_path} #{test_path} #{num_groups}:\n#{result}"
  end
end



output = type.map do |t|
  TestRun.from_run(ARGV[0], ARGV[1],t)
end

puts output.join("\n\n")
