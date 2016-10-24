#!/usr/bin/env ruby
require 'open3'

num_files = (1..7).to_a.reverse
type = (1..4).to_a

class TestRun < Struct.new(:result, :num_files, :type)
  def self.from_run(num_files, type, list_path)
    # this lets me the script without an argument
    if list_path.nil?
      list_path = "test_files/given/pa5/correctfiles.txt"
    end
    cmd = ["./PA7", list_path, num_files.to_s, type.to_s]
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
    self.new(output, num_files, type)
  end

  def to_s
    "./PA6 LIST #{num_files} #{type}:\n#{result}"
  end
end




output = type.map do |t|
  num_files.map do |n|
    TestRun.from_run(n, t, ARGV[0])
  end
end

puts output.map{|e| e + ["\n\n"]}.join
