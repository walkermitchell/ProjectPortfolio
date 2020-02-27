using System;


namespace Equipment
{
	public interface Equipment          //Equipment interface
	{
		Equipment equip();

        bool isWeap();
	}


	class Weapon : Equipment            //Weapon equipment type
	{

		private int power;

		public Weapon(int pow)          //Constructor
		{
			power = pow;
		}

		public int getPower()           //Getter for the weapon's offensive strength
		{
			return power;
		}

		public Equipment equip()        //Equip the weapon
		{
			return this;
		}

        public bool isWeap() => true;   //This is a weapon
    }

	class Armor : Equipment             //Armor equipment type
	{

		private int toughness;

		public Armor(int tough)         //Constructor
		{
			toughness = tough;
		}

		public int getToughness()       //Getter for the armor's defensive strength
		{
			return toughness;
		}

		public Equipment equip()        //Equip the armor
		{
			return this;
		}

        public bool isWeap() => false;  //This is not a weapon
	}
}


