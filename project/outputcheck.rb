require 'set'
`./PA10 test_files/perceptron_train/input_2.txt test_files/perceptron_test/test.txt 2 > test.txt`

sets1 = IO.binread("test.txt").split(/-+/).map{|x| x.split(/\s+/).to_set}
s2 = IO.binread("test_files/perceptron_test/test.txt").split(/\s+/).to_set

sets1.each_with_index do |s, i|
  puts "FOR CASE: #{i}"
  puts "Equal? #{s == s2}"
  puts "Missing? #{(s2 - s).inspect}"
  puts "Extra? #{(s - s2).inspect}"
  puts "---"
end
