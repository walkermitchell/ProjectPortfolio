
using System;

using Equipment;
using Enemy;

namespace Player
{
    class Player
    {
        private int maxHealth;
        private int currentHealth;
        private Weapon weap;                                //Currently equipped weapon
        private Armor arm;                                  //Currently equipped armor

        public Player()                                     //Constructor
        {
            maxHealth = 10;
            currentHealth = maxHealth;
            weap = null;
            arm = null;
        }

        public void attack(Enemy.Enemy enemy)               //Attack the enemy
        {
            int dmg = getWeapPow() / enemy.getArmTough();   //Damage dealt equals offensive power divided by defensive toughness, rounded down

            Console.WriteLine("You hit for " + dmg + " damage!");

            enemy.takeDamage(dmg);

            Console.WriteLine("They have " + enemy.getCurrentHealth() + " hp remaining!");
        }

        public void takeDamage(int dmg)                     //Get attacked by the enemy
        {
            currentHealth -= dmg;
        }

		public int getWeapPow()                             //Getter for the offensive strength of the currently equipped weapon
        {
            if(weap == null)                                //If no weapon is equipped, power is 1
            {
                return 1;
            }

			else return weap.getPower();
		}

		public int getArmTough()                            //Getter for the defensive strength of the currently equipped armor
        {
            if(arm == null)                                 //If no armor is equipped, toughness is 1 to prevent division by 0
            {
                return 1;
            }

			else return arm.getToughness();
		}

        public int getCurrentHealth()                       //Getter for currently remaining health
        {
            return currentHealth;
        }
        
        public void take(Equipment.Equipment equipment)     //Equip a piece of equipment
        {
            if (equipment.isWeap())
            {
                weap = (Weapon)equipment.equip();
            }

            else arm = (Armor)equipment.equip();

        }

        public void fullHeal()                              //Heal to full health
        {
            currentHealth = maxHealth;
        }
    }
}