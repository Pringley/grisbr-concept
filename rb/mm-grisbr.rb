require 'json'
require 'open3'

class File
  def read_toks; readline.strip.split; end
  def read_ints; read_toks.map(&:to_i); end
  def read_floats; read_toks.map(&:to_f); end
  def read_matrix rows; rows.times.collect { read_floats }; end
end

module Grisbr
  def self.multiply a, b
    args = JSON.generate({a: a, b: b})
    result, status = Open3.capture2("python grisbr-receiver.py",
                                    stdin_data: args)
    JSON.parse(result)["c"]
  end
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
Grisbr.multiply(a, b)
