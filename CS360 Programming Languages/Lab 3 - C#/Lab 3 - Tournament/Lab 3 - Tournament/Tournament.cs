using System;

using Player;
using Enemy;
using Equipment;



namespace Lab_3___Tournament
{
    class Tournament
    {
        public int currentRound;                                                            //Tracks the current round of the tourney

        private Player.Player pc;                                                           //The player
        private Enemy.Enemy npc;                                                            //The enemy
        private Equipment.Equipment[] items;                                                //Available equipment

        public Tournament()                                                                 //Constructor
        {
            currentRound = 0;
            pc = new Player.Player();
            npc = null;
            items = new Equipment.Equipment[5];
        }

        private bool nextRoundFight()                                                       //Fight logic
        {
            currentRound++;                                                                 //Increment the round tracker
            npc = new Enemy.Enemy(currentRound * 5, currentRound, currentRound);            //Spawn the enemy

            Console.WriteLine("Round " + currentRound + ", FIGHT!");

            while ((pc.getCurrentHealth()>0) && (npc.getCurrentHealth() > 0))               //Player and enemy hit each other until one dies
            {
                pc.attack(npc);
                npc.attack(pc);

                Console.WriteLine("Press ENTER to continue.");
                Console.ReadKey();
            }

            if(pc.getCurrentHealth() <= 0)                                                  //If player dies, they lose and the game ends
            {
                Console.WriteLine("You're dead! Better luck next time!");
                return false;
            }

            else                                                                            //If enemy dies, player moves on to next round
            {
                Console.WriteLine("You win! Good luck in the next round!");
                return true;
            }
        }

        private string betweenRounds()                                                      //Logic for between fights
        {
            Random rnd = new Random();
            pc.fullHeal();                                                                  //Heal the player

            items[0] = new Weapon(currentRound + rnd.Next(1, 5));                           //Spawn three randomly powered weapons
            items[1] = new Weapon(currentRound + rnd.Next(1, 5));
            items[2] = new Weapon(currentRound + rnd.Next(1, 5));
            items[3] = new Armor(currentRound + rnd.Next(1, 5));                            //Spawn two randomly powered armors
            items[4] = new Armor(currentRound + rnd.Next(1, 5));

            Console.WriteLine("Choose your Weapon:");                                       //Player chooses one of the three weapons
            Console.WriteLine("Type 0 to take the sword,");
            Console.WriteLine("Type 1 to take the axe,");
            Console.WriteLine("Or type 2 to take the spear.");
            string choose = Console.ReadLine();

            if(choose == "0")
            {
                Console.WriteLine("You have chosen the sword.");
                pc.take(items[0]);
                Console.WriteLine("This one has power " + pc.getWeapPow() + ".");
            }
            else if (choose == "1")
            {
                Console.WriteLine("You have chosen the axe.");
                pc.take(items[1]);
                Console.WriteLine("This one has power " + pc.getWeapPow() + ".");
            }
            else if (choose == "2")
            {
                Console.WriteLine("You have chosen the spear.");
                pc.take(items[2]);
                Console.WriteLine("This one has power " + pc.getWeapPow() + ".");
            }
            else { Console.WriteLine("You have chosen to use your fists."); }               //If player does not choose a weapon they fight unarmed

            Console.WriteLine("Choose your Armor:");                                        //Player chooses one of the two armors
            Console.WriteLine("Type 3 to take the leather armor,");
            Console.WriteLine("Or type 4 to take the chainmail.");
            choose = Console.ReadLine();

            if (choose == "3")
            {
                Console.WriteLine("You have chosen the leather armor.");
                pc.take(items[3]);
                Console.WriteLine("This one has toughness " + pc.getArmTough() + ".");
            }
            else if (choose == "4")
            {
                Console.WriteLine("You have chosen the chainmail.");
                pc.take(items[4]);
                Console.WriteLine("This one has toughness " + pc.getArmTough() + ".");
            }
            else { Console.WriteLine("You have chosen to wear no armor."); }                //If player does not choose an armor they fight unarmored

            Console.WriteLine("Press Y to continue to the next fight or N to give up.");    //Give player the option to exit
            string cont = Console.ReadLine();
            return cont;
        }


        static void Main(string[] args)
        {

            Tournament theGrandTourney = new Tournament();                                  //Initialize tournament


            string keepGoing = theGrandTourney.betweenRounds();
            bool winning = true;

            while (keepGoing != "N" && keepGoing != "n")                                    //Run logic until player dies or gives up
            {
                winning = theGrandTourney.nextRoundFight();
                if (!winning) break;
                keepGoing = theGrandTourney.betweenRounds();
            }


            Console.WriteLine("Press any key to exit.");                                    //Exit the program
            Console.ReadKey();

        }
    }


    

}