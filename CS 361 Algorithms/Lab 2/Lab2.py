
# CS361 Lab 2 
# Walker Mitchell

import time
import math
#import sys

# I'm using the same logic from Lab 1 for checking whether the array is sorted.
# Since this code was proven to work in Lab 1, I can use it here for Task 3.
def flgIsSorted(arr):
    if len(arr) <= 1:                             # When the array has only one element, it is sorted.
        return True
        
    if flgIsSorted(arr[:(len(arr)//2)]):                    # If the first half is sorted,  
        if flgIsSorted(arr[(len(arr)//2):]):                # the second half is sorted,
            if arr[(len(arr)//2)] >= arr[(len(arr)//2)-1]:  # and the first element of the second half is not smaller than the last element in the first half,
                return True                                 # the array is sorted.
    
    return False                                  # If the above conditions are not met, the array is not sorted.


# Quick Sort logic imported from Lab 1 for Task 4.
def auxQuickSort(arr, startIndex, endIndex):
    if startIndex < endIndex:
        partitionIndex = auxPartition(arr, startIndex, endIndex)
        auxQuickSort(arr, startIndex, partitionIndex)
        auxQuickSort(arr, (partitionIndex + 1), endIndex)
        
def auxPartition(arr, startIndex, endIndex):
    midPoint = (endIndex + startIndex)//2
    x = (arr[endIndex] + arr[startIndex] + arr[midPoint])//3
    i = startIndex-1
    for j in range(startIndex, endIndex+1):       # Pseudocode says start to end-1 but the final element is no longer
            if arr[j] <= x:                       # the pivot so we want to include arr[endIndex]. Also range excludes
                i += 1                            # the second parameter so we put endIndex+1 so arr[endIndex] is included.
                temp = arr[i]
                arr[i] = arr[j]
                arr[j] = temp
    return i
            

# Task 1: Radix Sort
# The book's pseudocode is totally useless. I'm using https://www.geeksforgeeks.org/radix-sort/ as my source.
def radixSort(arr):
    digit = 1
    biggest = max(arr)
    while(digit < biggest):
        countingSort(arr, digit)
        digit *= 10

# Counting Sort logic based on the above geeksforgeeks source. 
# I can't use Quicksort here because Radix needs a stable secondary sort.
def countingSort(arr, digit):
    length = len(arr)
    sorted = [0] * length       # Initialize output array
    count = [0] * 10            # Initialize counter array
    
    for i in range(0, length):  # Fill counter array
        index = arr[i]//digit
        count[index%10] += 1
    
    for i in range(1, 10):      # Set counter array to be filled with relevant indices
        count[i] += count[i-1]
        
    j = length - 1
    while j >= 0:               # Fill output array
        index = arr[j]//digit
        sorted[count[index%10] -1] = arr[j]
        count[index%10] -= 1
        j -= 1
        
    for i in range(0,length):   # Overwrite input array with sorted output array
        arr[i] = sorted[i]
        
# Task 2: Bucket Sort
# Using the book's pseudocode.
def bucketSort(arr):
    outside = []
    inside = []
    lowest = min(arr)
    numBuckets = int(math.sqrt(max(arr)- lowest))
    for i in range(numBuckets):         # Set up buckets
        outside.append(inside)
    
    bigBucket = (numBuckets*numBuckets)+lowest
    for k in arr:           # Fill buckets
        if k >= (bigBucket):        # Handles cases where the else logic would place the element out of range
            outside[-1] = outside[-1] + [k]
        else:
            outside[(k-lowest)//numBuckets] = outside[(k-lowest)//numBuckets] + [k]
        
    #for i in range(numBuckets):        # Just checking that the buckets filled properly
    #    print(len(outside[i]))
    
    for i in range(numBuckets):         # Sort buckets
        auxQuickSort(outside[i], 0, len(outside[i])-1)
    
    sorted = []
    for i in range(numBuckets):         # Concatenate buckets into sorted list
        sorted += outside[i]
    return sorted

# Task 6: Ten largest values
# Doing this one all by myself like a big boy.
def topTen(arr):
    if len(arr) <= 10:                  # If the array has ten values or less then just sort the array and we're done.
        auxQuickSort(arr, 0, len(arr)-1)
        return arr[::-1]                # Sort logic outputs in increasing order so we list it backwards.
    
    step = int(math.sqrt(len(arr)))
    temp = findTopTen(arr, 0, step)    # Returns a list of the 10 largest elements from each of the sqrt(len(arr)) pieces
    
    auxQuickSort(temp, 0, len(temp)-1)  # Sort the "candidates" for largest pieces
    temp = temp[len(temp)-10:len(temp)] # Take top ten
    return temp[::-1]                   # Return the top ten, backwards

def findTopTen(arr, index, step):
    if index >= step*step:                          # Stop when we run out of array
        auxQuickSort(arr, index, len(arr)-1)        # Sort what's left
        if (len(arr) - step*step) <= 10:            # If there are 10 or fewer elements left then return what we have
            return arr[step*step:len(arr)]
        else: return arr[len(arr)-10:len(arr)]       # Otherwise return largest 10
    else:
        auxQuickSort(arr, index, index+step-1)      # Quick Sort pieces
        return arr[index+step-10:index+step] + findTopTen(arr, index+step, step)    # Concatenate 10 largest with result of next level

# Lab 1 file loading logic for Task 6 (also every other task that needs info from the data file.)
# Initialize data array
data = []

# Open the data file
file = open("lab2_data.txt", "r")

# Iterate through the file line by line, and fill the array with the values from the file.
for line in file:
    if line != "\n":
        data.append(int(line.strip("n\'")))

# Close the file now that we're done with it
file.close()

#print("Using:")
#testData = data[:15]
#print(testData)

#print("Testing topTen")
#print(topTen(testData))

#print("Compare to:")
#auxQuickSort(testData, 0, len(testData)-1)
#testData.reverse()
#print(testData[:10])

#sortedData = bucketSort(testData)
#print("Does my Bucket Sort work?")
#print(flgIsSorted(sortedData))
#print(sortedData)

#radixSort(testData)
#print("Does my Radix Sort work?")
#print(flgIsSorted(testData))
#print(testData)



print("Now we're going to compare how long it takes Radix Sort vs. Bucket Sort vs. Quick Sort to sort arrays of increasing size.")
print("Then we will compare my topTen algorithm against sorting with QuickSort then manually printing the ten largest values.")
nums = len(data)
#sys.setrecursionlimit(nums)
arraySize = 1000
while arraySize <= nums:
    print("Starting with Radix Sort, sorting with size " + str(arraySize))
    radixData = data[:arraySize]
    timer = time.time()
    radixSort(radixData)
    timer = (time.time() - timer) * 1000
    print("Radix Sort took " + str(timer) + " milliseconds.")

    print("Did it sort correctly?")
    print(flgIsSorted(radixData))

    print("Now let's try using Bucket Sort.")

    bucketData = data[:arraySize]
    timer = time.time()
    bucketData = bucketSort(bucketData)
    timer = (time.time() - timer) * 1000
    print("Bucket Sort took " + str(timer) + " milliseconds.")

    print("Did it sort correctly?")
    print(flgIsSorted(bucketData))
    
    print("Now let's compare those against Quick Sort.")

    quickData = data[:arraySize]
    timer = time.time()
    auxQuickSort(quickData, 0, len(quickData)-1)
    timer = (time.time() - timer) * 1000
    print("Quick Sort took " + str(timer) + " milliseconds.")
    
    print("Now let's find the top ten.")
    testData = data[:arraySize]

    print("Testing topTen")
    timer = time.time()
    print(topTen(testData))
    timer = (time.time() - timer) * 1000
    print("topTen took " + str(timer) + " milliseconds.")

    print("Compare to:")
    timer = time.time()
    auxQuickSort(testData, 0, len(testData)-1)
    testData.reverse()
    timer = (time.time() - timer) * 1000
    print(testData[:10])
    print("Quick Sorting to find top ten took " + str(timer) + " milliseconds.")
    print("")
   
    arraySize *= 10

# The given data file has 9,999,097 numbers in it instead of 10,000,000 
# so I'm doing the final iteration across the entire file manually
print("Starting with Radix Sort, sorting with size 9,999,097")
radixData = data
timer = time.time()
radixSort(radixData)
timer = (time.time() - timer) * 1000
print("Radix Sort took " + str(timer) + " milliseconds.")

print("Did it sort correctly?")
print(flgIsSorted(radixData))

print("Now let's try using Bucket Sort.")

bucketData = data
timer = time.time()
bucketData = bucketSort(bucketData)
timer = (time.time() - timer) * 1000
print("Bucket Sort took " + str(timer) + " milliseconds.")

print("Did it sort correctly?")
print(flgIsSorted(bucketData))
    
print("Now let's compare those against Quick Sort.")

quickData = data
timer = time.time()
auxQuickSort(quickData, 0, len(quickData)-1)
timer = (time.time() - timer) * 1000
print("Quick Sort took " + str(timer) + " milliseconds.")
print("")
    
print("Now let's find the top ten.")
testData = data

print("Testing topTen")
timer = time.time()
print(topTen(testData))
timer = (time.time() - timer) * 1000
print("topTen took " + str(timer) + " milliseconds.")

print("Compare to:")
timer = time.time()
auxQuickSort(testData, 0, len(testData)-1)
testData.reverse()
timer = (time.time() - timer) * 1000
print(testData[:10])
print("Quick Sorting to find top ten took " + str(timer) + " milliseconds.")






