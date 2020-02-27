
-- Walker Mitchell's Lab 4

-- Problem 1

-- 1 liter == 0.264172 gallons
litersToGallons x = 0.264172*x

-- As of 9 March 2019, 1 CAD == 0.75 USD
cadToUSD x = 0.75*x

-- Actual word problem
gallons x = litersToGallons x
usd x = cadToUSD x
price cad liters = (usd cad)/(gallons liters)

-- Answer: 62.3 liters for 78.4 CAD converts to roughly $3.57 USD per gallon.


-- Problem 2

flightDistance :: (Double, Double) -> (Double, Double) -> Double
flightDistance (lat1, lon1) (lat2, lon2) = 3963*acos ((cos (degToRad lat1))*(cos (degToRad lat2))*(cos (degToRad (lon1 - lon2))) + (sin (degToRad lat1))*(sin (degToRad lat2)))
        where degToRad x = (pi/180)*x
        
-- Answer: The distance from (45,122) to (21,158) is 2631.8417 nautical miles.


-- Problem 3

bigOddCubes = sum [x^3 | x <- [1001,1003..1999]] 

-- Answer: 1874999250000 is the sum of the cubes of all the odd numbers between 1000 and 2000.


-- Problem 4

noSpaces string = filter (/= ' ') string
noEvens list = filter odd list
doubleIt list = map (2*) list
shfiftyShfife list = any (55==) list
oddsOnly list = all odd list


-- Problem 5
primes = checkFactors (2 : [3, 5..])
  where
    checkFactors (a:b) = a : checkFactors [x | x <- b, x `mod` a /= 0]

isPrime :: Integer -> Bool
isPrime x
    | null [ n | n <- [2..(x-1)], x `mod` n == 0 ] = True
    | otherwise = False

bigPrimes = [primes !! x | x <- [999..1019]]

-- The 1000th through 1020th primes are:
-- [7919,7927,7933,7937,7949,7951,7963,7993,8009,8011,8017,8039,8053,8059,8069,8081,8087,8089,8093,8101,8111]


-- Problem 6

primeFactors  :: Integer -> [Integer]
primeFactors 1 = []
primeFactors n
  | factors == []  = [n]
  | otherwise = factors ++ primeFactors (n `div` (head factors))
  where factors = take 1 $ filter (\x -> (n `mod` x) == 0) [2 .. n-1]

elimDuplicates :: Eq a => [a] -> [a]
elimDuplicates [] = []
elimDuplicates (x:xs)
    | x `elem` xs = elimDuplicates xs
    | otherwise = x : elimDuplicates xs

factor x = elimDuplicates (primeFactors x)



-- Problem 7

main=do
        
        print "Problem 1"
        print "62.3 liters for 78.4 CAD converts to:"
        print (price 78.4 62.3)
        print "USD per gallon."
        
        print "Problem 2"
        print "The distance from (45,122) to (21,158) is:"
        print (flightDistance (45,122) (21,158))
        print "nautical miles."
        
        print "Problem 3"
        print "The sum of the cubes of all the odd numbers between 1000 and 2000 is:"
        print bigOddCubes
        
        print "Problem 4"
        print (noSpaces "This is a sentence after the spaces have been removed.")
        print "Filtering even numbers out from the numbers 1 through 10, leaving only odds:"
        print (noEvens [1..10])
        print "Doubling the numbers 1 through 5:"
        print (doubleIt [1..5])
        print "True or False: Does the list [1,2,3,4,55] contain 55?"
        print (shfiftyShfife [1,2,3,4,55])
        print "True or False: Does the list [1,3,5,7] contain only odd numbers?"
        print (oddsOnly [1,3,5,7])
        
        print "Problem 5"
        print "The 1000th through 1020th primes are:"
        print bigPrimes
        
        print "Problem 6"
        print "The prime factors of 175561 and 62451532000 are:"
        print (factor 175561)
        print "and"
        print (factor 62451532000)
        
        print "Problem 7"
        print "This is a main function that prints all of my function outputs."






