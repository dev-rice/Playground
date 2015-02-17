from random import *

message = raw_input("Enter a message: ")

message_ints = map(lambda m: ord(m), message)
key_ints = map(lambda x: randint(0, 255), message_ints)
key = map(lambda e: hex(e) + ' ', key_ints)
key = ''.join(key)

encrypted_ints = map(lambda m, k: m ^ k, message_ints, key_ints)
encrypted = map(lambda e: hex(e) + ' ', encrypted_ints)
encrypted = ''.join(encrypted)

print "Key: %s" % key
print "Encrypted Message: %s" % encrypted

decrypted_ints = map(lambda e, k: e ^ k, encrypted_ints, key_ints)
decrypted = map(lambda d: chr(d), decrypted_ints)
decrypted = ''.join(decrypted)

print "Decrypted Message: %s" % decrypted
