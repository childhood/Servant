require 'digest'

File.open("test_file", "w") do |f|
	for i in 0..10 do
		password = Digest::MD5.hexdigest "user#{i}"
		f.write "user#{i}:#{password}\n"
	end
end
