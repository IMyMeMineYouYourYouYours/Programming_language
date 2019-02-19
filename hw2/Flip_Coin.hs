flipCoin' :: String -> Integer -> [Integer]
flipCoin' [] a = [0]
flipCoin' [x] a
	| x=='H' = [0]
	| x=='T' = [a+1,0]
flipCoin' (x:xs) a
	| x==y = flipCoin' xs (a+1)
	| otherwise = [a+1] ++ flipCoin' xs (a+1)
	where y= head xs


isCorrectChar :: String->Bool
isCorrectChar []=False
isCorrectChar [x]
	|x=='H'||x=='T' = True
	|otherwise = False
isCorrectChar (x:xs) 
	|x=='H'= x=='H'&&isCorrectChar xs
	|x=='T'= x=='T'&&isCorrectChar xs
	|otherwise = False

flipCoin :: String -> [Integer]
flipCoin xs =
	if isCorrectChar xs then flipCoin' xs 0 else error("Input error\n") 
