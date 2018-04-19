package com.company;

import java.util.concurrent.Semaphore;

public class Main
{
    private static Semaphore semCons = new Semaphore(0);
    private static Semaphore semProd = new Semaphore(1);

    public static void main(String[] args)
    {
        Thread producer = new Thread(new Producer(semCons,semProd));
        Thread consumer = new Thread(new Consumer(semCons,semProd));
        producer.start();
        consumer.start();
    }


}
class Producer implements Runnable
{
    private Semaphore semCons;
    private Semaphore semProd;

    Producer(Semaphore semCon, Semaphore semPro)
    {
        semCons= semCon;
        semProd= semPro;
    }

    @Override
    public void run()
    {
        while(true)
        {
            try
            {
                semProd.acquire();
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            System.out.println("Produced.");
            try
            {
                Thread.sleep(1000);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            semCons.release();
        }
    }
}

class Consumer implements Runnable
{
    private Semaphore semCons;
    private Semaphore semProd;

    Consumer(Semaphore semCon, Semaphore semPro)
    {
        semCons= semCon;
        semProd= semPro;
    }

    @Override
    public void run()
    {
        while(true)
        {
            try
            {
                semCons.acquire();
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            System.out.println("Consumed.");
            try
            {
                Thread.sleep(1000);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            semProd.release();
        }
    }
}
