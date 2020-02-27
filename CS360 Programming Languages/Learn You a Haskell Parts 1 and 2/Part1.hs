
--Babby's first Haskell function
doubleMe x = x + x

--Problem 1
squareRoot x = sqrt (fromIntegral x)

--Problem 2
beforeA = pred 'A'

--Problem 3
isOdd x = even (3*x + 1)

--Problem 4
prodOdds1To99 = product [1,3..99]

--Problem 5
largestInMiddle = maximum (init (tail [99,23,4,2,67,82,49,-40]))

--Problem 6
constructList = 6:19:41:(-3):[]

--Problem 7
twentySevenEvens = take 27 [2,4..]

--Problem 8
oddThreeSeven = [21,63..200]

--Problem 9
oddNines = length [117, 135..200]

--Problem 10
countNegs x = length [a | a <- x, a < 0]

--Problem 11
hexMap = zip [0..15] (['0'..'9'] ++ ['A'..'F'])

--Problem 12
makeList x = [ [1..a] | a <- [1..x], x > 0 ]

--Problem 13
sanitize x = concat [if a == ' ' then "%20" else [a] | a <- x]
    
--Problem 14
--(*) works with ints, integers, doubles, floats, etc.
--min works with ints, integers, doubles, floats, etc.
--length is a Foldable, which is not listed on the given type class heirarchy"
--sqrt works with floats and doubles
--(>) works with ints, integers, doubles, floats, etc.

--Problem 15
getSuit :: Int -> String
getSuit 0 = "Heart"
getSuit 1 = "Diamond"
getSuit 2 = "Spade"
getSuit 3 = "Club"
getSuit x = "0-3 only!"

--Problem 16
dotProduct :: (Double,Double,Double) -> (Double,Double,Double) -> Double
dotProduct (a,b,c) (x,y,z) = (a*x)+(b*y)+(c*z)

--Problem 17
reverseFirstThree :: [a] -> [a]
reverseFirstThree [] = []
reverseFirstThree [a] = [a]
reverseFirstThree [a,b] = [b,a]
reverseFirstThree [a,b,c] = [c,b,a]
reverseFirstThree x = (x !! 2):(x !! 1):(x !! 0):tail (tail (tail x))

--Problem 18
feelsLike :: Double -> String
feelsLike temp
    | temp <= (-40) = "Frostbite Central!"
    | temp <= 0     = "T-shirt weather in Alaska!"
    | temp <= 32    = "It's below freezing!"
    | temp <= 60    = "Nice and brisk!"
    | otherwise     = "Sun's out guns out!"

--Problem 19
feelsLike2 :: Double -> (Double,String)
feelsLike2 temp
    | temp <= (-40) = (fahr,"Frostbite Central!")
    | temp <= (-18) = (fahr,"T-shirt weather in Alaska!")
    | temp <= 0     = (fahr,"It's below freezing!")
    | temp <= 16    = (fahr,"Nice and brisk!")
    | otherwise     = (fahr,"Sun's out guns out!")
    where fahr = (1.8*temp) + 32

--Problem 20
cylinderToVolume :: [(Double,Double)] -> [Double]
cylinderToVolume list = [ volume | (r,h) <- list, let volume = pi * (r ^ 2) * h ]


--Test ALL the functions!
main=do
    print "Problem 1" 
    print (squareRoot 818281336460929553769504384519009121840452831049)
    
    print "Problem 2"
    print beforeA
    
    print "Problem 3"
    print "Trying with x = 5:"
    print (isOdd 5)
    print "Trying with x = 8:"
    print (isOdd 8)
    
    print "Problem 4"
    print prodOdds1To99
    
    print "Problem 5"
    print largestInMiddle
    
    print "Problem 6"
    print constructList
    
    print "Problem 7"
    print twentySevenEvens
    
    print "Problem 8"
    print oddThreeSeven
    
    print "Problem 9"
    print oddNines
    
    print "Problem 10"
    print (countNegs [-4,6,7,8,-14])
    
    print "Problem 11"
    print hexMap
    
    print "Problem 12"
    print "Trying with x = 3:"
    print (makeList 3)
    print "Trying with x = 5:"
    print (makeList 5)
    print "Trying with x = -2:"
    print (makeList (-2))
    
    print "Problem 13"
    print (sanitize "http://wou.edu/my homepage/I love spaces.html")
    
    print "Problem 15"
    print (getSuit 17)
    print (getSuit 0)
    print (getSuit 1)
    print (getSuit 2)
    print (getSuit 3)
    
    print "Problem 16"
    print (dotProduct (1,2,3.0) (4.0,5,6))
    
    print "Problem 17"
    print (reverseFirstThree [1,2])
    print (reverseFirstThree [1,2,3,4,5,6])
    
    print "Problem 18"
    print "Trying with -45.3"
    print (feelsLike (-45.3))
    print "Trying with -3"
    print (feelsLike (-3))
    print "Trying with 30"
    print (feelsLike 30)
    print "Trying with 45"
    print (feelsLike 45)
    print "Trying with 95"
    print (feelsLike 95)
    
    print "Problem 19"
    print "Trying with ~-45.3 Fahrenheit"
    print (feelsLike2 (-42.9444))
    print "Trying with ~-3 Fahrenheit"
    print (feelsLike2 (-19.4444))
    print "Trying with ~30 Fahrenheit"
    print (feelsLike2 (-1.1111))
    print "Trying with ~45 Fahrenheit"
    print (feelsLike2 7.2222)
    print "Trying with ~95 Fahrenheit"
    print (feelsLike2 35)
    
    print "Problem 19"
    print "Trying with [(2,5.3),(4.2,9),(1,1),(100.3,94)]"
    print (cylinderToVolume [(2,5.3),(4.2,9),(1,1),(100.3,94)])
    
    
    
    
    
    
    