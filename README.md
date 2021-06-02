
# mosher

datamosher mainly designed for AVI video

![example with n=65 d=20 m=25](https://i.imgur.com/9ERNIYD.png)

(although any file will work, it is tuned towards getting decent looking results from low quality AVI video)


use in command line
```cpp
mosh example.avi h=7000 n=30 d=10 m=5
```

where the parameters are

h = header length (in bytes)

n = strength of the noise algorithm

d = strength of the duplicate algorithm

m = strengths of the move algorithm


for algorithm strength, 0 means not doing anything and 100 is likely to make the file into garbage for tested AVI video

you can call it with multiple file paths and itll run the mosh with the same parameters on all of them 

not optimised in any way and loads the whole file to memory (so don't throw in a whole movie)
