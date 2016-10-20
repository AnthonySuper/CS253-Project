num_files = (1..7).to_a.reverse
type = (1..3).to_a

class TestRun < Struct.new(:result, :num_files, :type)
  def to_s
    res = result.lines.map do|l| 
      "\t#{l.gsub("\t"," ")}"
    end.join("")
    "./PA6 LIST #{num_files} #{type}\n#{res}"
  end
end

def run_file(num_files, type)
  prefix = "test_files/given/pa5"
  output = `./PA6 #{prefix}/correctfiles.txt #{num_files} #{type}`
  output.gsub(prefix, "")
end


output = type.map do |t|
  num_files.map do |n|
    TestRun.new(run_file(n, t), n, t)
  end
end

puts output.join("\n")
