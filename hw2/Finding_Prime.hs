usqrt::Int->Int
usqrt=floor.sqrt.fromIntegral

divide :: Int->[Int]
divide a = [x|x<-[1..usqrt(a)],a `mod` x ==0]

isPrime::Int -> Bool
isPrime 1=False
isPrime a= divide a ==[1]

findingPrimes::Int -> Int -> [Int]
findingPrimes a 0 = []
findingPrimes a b=take b (filter isPrime [a..])
