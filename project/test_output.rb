require 'open3'
num_files = (1..7).to_a.reverse
type = (1..4).to_a

class TestRun < Struct.new(:result, :num_files, :type)
  def to_s
    res = result.lines.map do|l| 
      "\t#{l.gsub("\t"," ")}"
    end.join("")
    "./PA6 LIST #{num_files} #{type}\n#{res}"
  end
end

def run_file(num_files, type)
  prefix = "test_files/given/pa5/"
  cmd = ["./PA6", "#{prefix}correctfiles.txt", num_files.to_s, type.to_s]
  stdin, stdout, stderr, wait_thr = Open3.popen3(*cmd)
  output = stdout.read
  if wait_thr.value != 0
    raise ["COMMAND FAILED (#{wait_thr.value}) ",
           "'#{cmd.join(" ")}' with msg #{stderr.read}"].join
  end
  stdin.close
  stdout.close
  stderr.close
  output.gsub(prefix, "")
end


output = type.map do |t|
  num_files.map do |n|
    TestRun.new(run_file(n, t), n, t)
  end
end

puts output.join("\n")
