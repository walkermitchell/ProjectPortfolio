
using System;

using Equipment;
using Player;

namespace Enemy
{
	class Enemy
    { 
		private int maxHealth;      
		private int currentHealth;
		private Weapon weap;                                        //Currently equipped weapon
		private Armor arm;                                          //Currently equipped armor

        public Enemy(int health, int weapon, int armor)             //Constructor
        {
            maxHealth = health;
            currentHealth = maxHealth;
            weap = new Weapon(weapon);
            arm = new Armor(armor);
        }

        public void attack(Player.Player player)                    //Attack the player
        {
            int dmg = getWeapPow() / player.getArmTough();          //Damage dealt equals offensive power divided by defensive toughness, rounded down

            Console.WriteLine("You got hit for " + dmg + " damage!");
            
            player.takeDamage(dmg);

            Console.WriteLine("You have " + player.getCurrentHealth() + " hp remaining!");
        }

        public void takeDamage(int dmg)                             //Get attacked by the player
        {
            currentHealth -= dmg;
        }

		public int getWeapPow()                                     //Getter for the offensive strength of the currently equipped weapon
		{
			return weap.getPower();
		}

		public int getArmTough()                                    //Getter for the defensive strength of the currently equipped armor
        {
			return arm.getToughness();
        }

        public int getCurrentHealth()                               //Getter for currently remaining health
        {
            return currentHealth;
        }


    }
}

