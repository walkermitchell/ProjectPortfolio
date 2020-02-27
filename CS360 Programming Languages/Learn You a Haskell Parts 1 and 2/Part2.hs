
--Learning Me a Haskell Part 2

import Assn2b

--Problem 1
gcdMine :: Integral a => a -> a -> a
gcdMine a 0 = a
gcdMine 0 b = b
gcdMine a b = (gcdMine b (a `mod` b))
    
gcdCheck x y = (myAnswer, correctAnswer, comment)
    where
    myAnswer        = gcdMine x y
    correctAnswer   = gcd x y
    comment         = if myAnswer == correctAnswer then "Matches" else "Does not Match"
    
--Problem 2
fibonacci :: Int -> Int
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = (fibonacci (n-1)) + (fibonacci (n-2))

--Problem 3
count :: (Eq a, Num b) => a -> [a] -> b
count a [] = 0
count a b = (if head b == a then 1 else 0) + (count a (tail b))

--Problem 4
sanitize :: String -> String
sanitize "" = ""
sanitize s  = (if head s == ' ' then "%20" else [head s]) ++ (sanitize (tail s))

--Problem 5
timesTen :: Num a => [a] -> [a]
timesTen a = map (*10) a

--Problem 6
increment :: Enum a => [a] -> [a]
increment a = map (succ) a

--Problem 7
theAnswerToLifeTheUniverseAndEverything :: Integral a => [a] -> Bool
theAnswerToLifeTheUniverseAndEverything a = any ( == 0) (map(`mod` 42) a)

--Problem 8
powersOfTen :: Integral a => [a] -> [a]
powersOfTen a = zipWith (^) (take (length a) (repeat 10)) a

--Problem 9
stringStripper :: String -> String
stringStripper s = reverse (dropWhile (==' ') (reverse s))

--Problem 10
allEven :: Integral a => [a] -> Bool
allEven a = all (even) a

--Problem 11
thatsANegatory ::  [String] -> [String]
thatsANegatory s = map ("not " ++) s

--Problem 12
sdrawkcab ::  [String] -> [String]
sdrawkcab s = map (reverse) s

--Problem 13
--plus :: Num a => a -> a -> a
--plus = \x y -> x + y

--Problem 14
--timesFour :: Num a => a -> a
--timesFour = \x -> x * 4

--Problem 15
--indexOne :: [a] -> a
--indexOne = \x -> x !! 1

--Problem 16
--roundRoot :: (RealFloat a, Integral b) => a -> b
---roundRoot = \x -> round (sqrt x)

--Problem 17
--splitString :: String -> [String]
--splitString = \x -> words x

--Problem 18
pyth :: RealFloat a => [(a,a)] -> [(a,a,a)]
pyth legs = map (\(x,y) -> (x,y,sqrt((x^2) + (y^2)))) legs

--Test ALL the functions!
main=do
        print "Problem 1" 
        print (gcdCheck 111 259)
        print (gcdCheck 2945 123042)
        print (gcdCheck (2*5*7) (7*23))
        
        print "Problem 2" 
        print [fibonacci n | n <- [0..20]]
        
        print "Problem 3" 
        print (count 7 [1,7,6,2,7,7,9])
        print (count 'w' "western oregon wolves")
        
        print "Problem 4" 
        print (sanitize "http://wou.edu/my homepage/I love spaces.html")
        
        print "Problem 5" 
        print (timesTen [0,1,2,3,4,5])
        
        print "Problem 6" 
        print (increment [0,1,2,3,4,5])
        print (increment "Hello")
        
        print "Problem 7" 
        print (theAnswerToLifeTheUniverseAndEverything [1,3,42])
        print (theAnswerToLifeTheUniverseAndEverything [1,3])
        
        print "Problem 8" 
        print (powersOfTen [5,3,8,2,3,6,3])
        
        print "Problem 9" 
        print (stringStripper "  Hi!   ")
        
        print "Problem 10" 
        print (allEven [2,4,6,8])
        print (allEven [2,4,6,9])
        
        print "Problem 11" 
        print (thatsANegatory ["Funny","cold","slow"])
        
        print "Problem 12" 
        print (sdrawkcab ["This", "is", "a", "sentence", "sdrawkcab", "racecar", "Aibohphobia"])
        
        print "Problem 13" 
        print ((\x y -> x + y) 2 3)
        
        print "Problem 14" 
        print ((\x -> x * 4) 10)
        
        print "Problem 15" 
        print ((\x -> x !! 1) [1,2,3,4])
        
        print "Problem 16" 
        print ((\x -> round (sqrt x)) 25.0)
        print ((\x -> round (sqrt x)) 15.0)
        print ((\x -> round (sqrt x)) 2.0)
        
        print "Problem 17" 
        print ((\x -> words x) "This is a sentence written in the usual way.")
        
        print "Problem 18" 
        print (pyth [(3,4),(5,16),(9.4,2)])
        
        print "Modules Problem 2"
        print (length' [1,2,3,4])
        
        print "Modules Problem 3"
        print "Left to Right:"
        print (convertIntToStringLeft [5,2,8,3,4])
        print "Right to Left:"
        print (convertIntToStringRight [5,2,8,3,4])
        
        print "Modules Problem 4"
        print "Testing Equivalency and Validity"
        print (length (filter (<20) [1..100]))
        print (length $ filter (<20)[1..100])
        print "Next"
        print (take 10 (cycle (filter (>5) (map (*2) [1..10]))))
        print (take 10 $ cycle $ filter (>5) $ map (*2) [1..10])

        
        
        
        
        