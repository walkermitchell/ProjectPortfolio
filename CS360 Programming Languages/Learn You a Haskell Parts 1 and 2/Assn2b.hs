
--Yay Modules!


--Problem 1
{-  Since foldl proceeds from left to right, the statement 'foldl (*) 6 [3,5,8]' would
    apply the operator such that the calculation would be 6*3*5*8=720. foldr, on the other
    hand, proceeds from right to left so the calculation for 'foldr (*) 6 [3,5,8]' would 
    be 6*8*5*3=720. Because multiplication is commutative, the operations are equivalent 
    in this case. However, if a different binary function was used, such as the prepend 
    operator :, the outputs of foldl and foldr may differ.                              -}

module Assn2b
(

length',
convertIntToStringLeft,
convertIntToStringRight

)where

import Data.Char

--Problem 2
length' :: [a] -> Int
length' a = foldl (\acc x -> acc + 1) 0 a

--Problem 3
convertIntToStringLeft :: [Int] -> [Char]
convertIntToStringLeft list = foldl (\acc x -> acc ++ ([intToDigit x])) "" list

convertIntToStringRight :: [Int] -> [Char]
convertIntToStringRight list = foldr (\x acc -> (intToDigit x):acc) "" list

--Problem 4
--length $ filter (<20)[1..100]
--take 10 $ cycle $ filter (>5) $ map (*2) [1..10]

