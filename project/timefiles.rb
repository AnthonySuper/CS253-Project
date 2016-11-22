#!/usr/bin/env ruby

path = "test_files/given/perceptron_train/correctfiles.txt"
op = []
[50, 25, 12, 6, 3].each do |num|
  str = "time ./PA9 #{path} #{path} #{num}"
  puts str
  output = `#{str}`
  puts output.lines.inspect
end

puts op.inspect
