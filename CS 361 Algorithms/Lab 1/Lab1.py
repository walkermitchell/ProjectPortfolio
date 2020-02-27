
# I've chosen to switch from C# to Python for this lab because Python has a much more robust file I/O system.

import time

# Task 2: Merge Sort
# I based these functions off of the logic at https://www.geeksforgeeks.org/merge-sort/.
# I used the example code there as a template, but wrote this code myself.
def auxMergeSort(arr, startIndex, endIndex):
    midPoint = (endIndex + startIndex)//2
    if endIndex > (startIndex+1): 
        auxMergeSort(arr, startIndex, midPoint)
        auxMergeSort(arr, midPoint, endIndex)
    merge(arr, startIndex, midPoint, endIndex)
    
# As of 4/19 this code has logic errors that result in duplicate array elements. Still not sure why.
# Update: Duplication issue is now fixed.
def merge(arr, startIndex, midPoint, endIndex):
    if startIndex == endIndex:
        return
    i = j = 0
    k = startIndex
    leftHalf = arr[startIndex:midPoint]
    rightHalf = arr[midPoint:endIndex]
    while i < len(leftHalf) and j < len(rightHalf): # Combining the halves
        if leftHalf[i] < rightHalf[j]:
            arr[k] = leftHalf[i]
            i += 1
        else:
            arr[k] = rightHalf[j]
            j += 1
        k += 1
    while i < len(leftHalf):                        # Checking for elements left over due to mismatched 
        arr[k] = leftHalf[i]                        # sizes of the left and right halves
        i += 1
        k += 1        
    while j < len(rightHalf): 
        arr[k] = rightHalf[j] 
        j += 1
        k += 1
            
# Task 3: Quick Sort
# This code is based off the pseudocode in the textbook.
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
    #temp = arr[i+1]                              # arr[endIndex] isn't the pivot so we don't need this final swap
    #arr[i+1] = arr[endIndex]
    #arr[endIndex] = temp
    return i

# Task 4: Sort Checker
# I wrote this function using only the prompt as a guide.
def flgIsSorted(arr):
    if len(arr) <= 1:                             # When the array has only one element, it is sorted.
        return True
        
    if flgIsSorted(arr[:(len(arr)//2)]):                    # If the first half is sorted,  
        if flgIsSorted(arr[(len(arr)//2):]):                # the second half is sorted,
            if arr[(len(arr)//2)] >= arr[(len(arr)//2)-1]:  # and the first element of the second half is not smaller than the last element in the first half,
                return True                                 # the array is sorted.
    
    return False                                  # If the above conditions are not met, the array is not sorted.

# Task 1: Loading the file
# Initialize array, array element counter, and array sum counter
data = []
nums = 0
sum = 0

# Open the data file
file = open("lab1_data.txt", "r")

# Iterate through the file line by line, and fill the array with the values from the file.
timer = time.time()
for line in file:
    if line != "\n":
        data.append(int(line.strip("n\'")))
        nums += 1 
        sum += data[nums-1]
timer = (time.time() - timer) * 1000

# Close the file now that we're done with it
file.close()

print("It took " + str(timer) + " milliseconds to read the file.")

#print(data)
print("This array is " + str(nums) + " elements long, and its elements add to " + str(sum) + ".")
#print("Let's try sorting the first 10 elements of this array using Merge Sort.")
#mergeData = data[:10]
#print("")
#print("Here's the first 10 elements of the data file:")
#print(mergeData)
#print("Testing sort checker: Is this array sorted right now?")
#print(flgIsSorted(mergeData))
#auxQuickSort(mergeData, 0, len(mergeData)-1)
#print("Now here they are after being sorted via Quick Sort:")
#print(mergeData)
#print("Testing sort checker: Is this array sorted right now?")
#print(flgIsSorted(mergeData))

# Task 5: Timing is everything

print("Now we're going to compare how long it takes Merge Sort vs. Quick Sort to sort arrays of increasing size.")
arraySize = 1000
while arraySize <= nums:
    print("Starting with Merge Sort, sorting with size " + str(arraySize))
    mergeData = data[:arraySize]
    timer = time.time()
    auxMergeSort(mergeData, 0, len(mergeData))
    timer = (time.time() - timer) * 1000
    print("Merge Sort took " + str(timer) + " milliseconds.")
    #print(mergeData)

    print("Did it sort correctly?")
    timer = time.time()
    print(flgIsSorted(mergeData))
    timer = (time.time() - timer) * 1000
    print("It took " + str(timer) + " milliseconds to check that.")

    print("Now let's try using Quick Sort.")

    quickData = data[:arraySize]
    timer = time.time()
    auxQuickSort(quickData, 0, len(quickData)-1)
    timer = (time.time() - timer) * 1000
    print("Quick Sort took " + str(timer) + " milliseconds.")
    print(quickData)

    print("Did it sort correctly?")
    timer = time.time()
    print(flgIsSorted(quickData))
    timer = (time.time() - timer) * 1000
    print("It took " + str(timer) + " milliseconds to check that.")
   
    arraySize *= 10

