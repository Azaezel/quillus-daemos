Armor =
{
	statmod = {"def",1},												-- when used/equiped adds to this stack(s)
	modifiedby = {"str",1}												-- when used/equiped checks this stack(s) for a multiplier
}

Cut =
{
	statmod = {"hp",-1},
	modifiedby = {"def",1,"dex",1}
}

Shoot =
{
	statmod = {"hp",-1},
	modifiedby = {"def",1,"con",1}
}

Confuse =
{
	statmod = {"mp",-5, "char",-3, "wis",-2, "int",-1},
	modifiedby = {"wis",1}
}

Poison =
{
	statmod = {"hp",-2, "str", -1,"con", -1},
	modifiedby = {"con",1}
}

Braaaains =
{
	statmod = {"int", 1},
	modifiedby = {"lvl",1}
}

EffectList = {
	[1] = Armor, 													-- key, item
	[2] = Cut,
	[3] = Shoot,
	[4] = Confuse,
	[5] = Poison,
	[6] = Braaaains
}