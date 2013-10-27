require 'matrix'

class File
  def read_toks; readline.strip.split; end
  def read_ints; read_toks.map(&:to_i); end
  def read_floats; read_toks.map(&:to_f); end
  def read_matrix rows; Matrix[*rows.times.collect { read_floats }]; end
end

if ARGV.empty?
  puts "Usage: #{$0} <input>"
  exit false
end

# Read matricies from file.
file = ARGV.first
a, b = File.open(file) do |fp|
  m, k, n = fp.read_ints
  [fp.read_matrix(m), fp.read_matrix(k)]
end

# Multiply.
c = a * b
