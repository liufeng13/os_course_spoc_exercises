#coding=utf-8
import threading
import random
import time

money = 0

class Save(threading.Thread):
    def __init__(self,threadName,semaphore):
       threading.Thread.__init__(self,name=threadName)
       self.threadSemaphore=semaphore
    def run(self):
       global money
       while True:
           self.threadSemaphore.acquire()
           money += 10
           print "%s save:now money is %s." %(self.getName(),money)
           self.threadSemaphore.release()
           time.sleep(random.randrange(1, 5))

class Load(threading.Thread):
    def __init__(self,threadName,semaphore):
       threading.Thread.__init__(self,name=threadName)
       self.threadSemaphore=semaphore
    def run(self):
       global money
       while True:
           self.threadSemaphore.acquire()
           if money>0:
               money -= 10
               print "%s load:now money is %s." %(self.getName(),money)
           else:
               print "%s load:no money now." %(self.getName())
           self.threadSemaphore.release()
           time.sleep(random.randrange(1, 5))

threads=[]
threadSemaphore=threading.Semaphore(1)
for i in range(1,6):
   threads.append(Save("save"+str(i),threadSemaphore))
for i in range(1,8):
    threads.append(Load("load"+str(i),threadSemaphore))
for thread in threads:
   thread.start()
