// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGamePlayTags.h"

namespace BaseGamePlayTags
{
	//Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Riding_Move,"InputTag.Riding.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Riding_Jump,"InputTag.Riding.Jump")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Riding_Sprint,"InputTag.Riding.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold_Spacebar, "InputTag.Hold.Spacebar")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Spacebar, "InputTag.Spacebar")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Backstep, "InputTag.Backstep")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold, "InputTag.Hold")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Hold.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "InputTag.Interact")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_AshOfWar,"InputTag.AshOfWar")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_AshOfWar_Left,"InputTag.AshOfWar.Left")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_AshOfWar_Right,"InputTag.AshOfWar.Right")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold_Shift, "InputTag.Hold.Shift")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Menu,"InputTag.Menu")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UseItem,"InputTag.UseItem")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget,"InputTag.SwitchTarget")

	//PlayerHorseRiding InputTag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Horse_LeftAttack,"InputTag.Horse.LeftAttack")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Horse_RightAttack,"InputTag.Horse.RightAttack")
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Right, "InputTag.Equip.Right")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Left, "InputTag.Equip.Left")

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Spell, "InputTag.Equip.Spell")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Item,"InputTag.Equip.Item")
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_LeftHand, "InputTag.Attack.LeftHand")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_RightHand, "InputTag.Attack.RightHand")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_RemoveWeapon_Primary, "Player.Event.RemoveWeapon.Primary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_RemoveWeapon_Secondary, "Player.Event.RemoveWeapon.Secondary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_RemoveWeapon, "Player.Event.RemoveWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AddWeapon_Primary, "Player.Event.AddWeapon.Primary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AddWeapon_Secondary, "Player.Event.AddWeapon.Secondary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AddWeapon, "Player.Event.AddWeapon")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Primary,"Player.Event.Equip.Primary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Secondary,"Player.Event.Equip.Secondary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnEquip_Primary,"Player.Event.UnEquip.Primary")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnEquip_Secondary,"Player.Event.UnEquip.Secondary")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right,"Player.Event.SwitchTarget.Right")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left,"Player.Event.SwitchTarget.Left")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Mount,"Player.Event.Mount")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnMount,"Player.Event.UnMount")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Use_Potion,"Player.Event.Use.Potion")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Success_Potion,"Player.Event.Success.Potion")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessBlock,"Player.Event.SuccessBlock")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Success_Spell,"Player.Event.Success.Spell")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Spawn_Effect,"Player.Event.Spawn.Effect")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Parry,"Player.Ability.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Left_Hand, "Player.Ability.Attack.Left.Hand")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Right_Hand, "Player.Ability.Attack.Right.Hand")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Left_Weapon, "Player.Ability.Attack.Light.Left.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Right_Weapon, "Player.Ability.Attack.Light.Right.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Left_Weapon, "Player.Ability.Attack.Heavy.Left.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Right_Weapon, "Player.Ability.Attack.Heavy.Right.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Weapon,"Player.Ability.Equip.Weapon")

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Groggy,"Player.Ability.Groggy")

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Horse_Jump,"Player.Ability.Horse.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Horse_Sprint,"Player.Ability.Horse.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_UseItem,"Player.Ability.UseItem")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_EquipItem,"Player.Ability.EquipItem")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Horse_Attack_Left,"Player.Ability.Horse.Attack.Left")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Horse_Attack_Right,"Player.Ability.Horse.Attack.Right")
	
	//Player Spell Tag
	UE_DEFINE_GAMEPLAY_TAG(Spell_Magic,"Spell.Magic")
	UE_DEFINE_GAMEPLAY_TAG(Spell_Pray,"Spell.Pray")
	

	//Player SetByCaller Init
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_MaxHP,"Player.SetByCaller.MaxHP")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_MaxFP,"Player.SetByCaller.MaxFP")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_MaxStamina,"Player.SetByCaller.MaxStamina")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_BaseAttack,"Player.SetByCaller.BaseAttack")
	
	//Player Stat
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_EquipWeapon_Right, "Player.SetByCaller.EquipWeapon.Right")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_EquipWeapon_Left, "Player.SetByCaller.EquipWeapon.Left")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_UnequipWeapon, "Player.SetByCaller.UnequipWeapon")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_StaminaCost, "Player.SetByCaller.StaminaCost")
	UE_DEFINE_GAMEPLAY_TAG(Player_Block_Stamina_Recovery, "Player.Block.Stamina.Recovery")

	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Right_Melee, "Player.SetByCaller.AttackType.Right.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Right_Magic, "Player.SetByCaller.AttackType.Right.Magic")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Left_Melee, "Player.SetByCaller.AttackType.Left.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Left_Magic, "Player.SetByCaller.AttackType.Left.Magic")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Charged,"Player.SetByCaller.AttackType.Charged")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Recovery_Hp, "Player.SetByCaller.Recovery.Hp")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Recovery_Fp, "Player.SetByCaller.Recovery.Fp")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Buff_Atk,"Player.SetByCaller.Buff.Atk")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Buff_Def,"Player.SetByCaller.Buff.Def")

	//Player SetByCaller Cost
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Cost_FP,"Player.SetByCaller.Cost.FP")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Cost_Stamina,"Player.SetByCaller.Cost.Stamina")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_LeftCost,"Player.SetByCaller.LeftCost")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_RightCost,"Player.SetByCaller.RightCost")
	
	
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_Targeting, "InputTag.Toggle.Targeting")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Targeting, "Player.Ability.Targeting")

	//Player Status
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Targeting, "Player.Status.Targeting")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Block, "Player.Status.Block")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Parry,"Player.Status.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Finisher,"Player.Status.Finisher")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Unbeatable,"Player.Status.Unbeatable")

	UE_DEFINE_GAMEPLAY_TAG(Equipment_Staff, "Equipment.Staff")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_SacredSeal, "Equipment.SacredSeal")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Shield, "Equipment.Shield")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Sword, "Equipment.Sword")
	
	//Player Items
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Weapon_Primary_AlabasterSword, "Equipment.Weapon.Primary.AlabasterSword")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Weapon_Primary_AlbinauricStaff, "Equipment.Weapon.Primary.AlbinauricStaff")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Shield_Secondary_Buckler, "Equipment.Shield.Secondary.Buckler")

	//Player Items Test
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Staff_AlbinauricStaff,"Equipment.Staff.AlbinauricStaff")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_SacredSeal_FingerSeal,"Equipment.SacredSeal.FingerSeal")
	
	//Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_S, "Enemy.Ability.Melee.S")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_M, "Enemy.Ability.Melee.M")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Turn_Left, "Enemy.Ability.Turn.Left")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Turn_Right, "Enemy.Ability.Turn.Right")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Turn_Back, "Enemy.Ability.Turn.Back")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Stabbed, "Enemy.Ability.Stabbed")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Parried, "Enemy.Ability.Parried")
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Idle, "Enemy.Status.Idle")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Battle, "Enemy.Status.Battle")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Stabbed, "Enemy.Status.Stabbed")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Phase2, "Enemy.Status.Phase2")
	
	//Boss
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Pattern_Left, "Enemy.Ability.Pattern.Left")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Pattern_Right, "Enemy.Ability.Pattern.Right")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Pattern_Back, "Enemy.Ability.Pattern.Back")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Phase2_Melee_S, "Enemy.Ability.Phase2.Melee.S")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Phase2_Melee_M, "Enemy.Ability.Phase2.Melee.M")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Phase2_Turn_Left, "Enemy.Ability.Phase2.Turn.Left")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Phase2_Turn_Right, "Enemy.Ability.Phase2.Turn.Right")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Phase2_Turn_Back, "Enemy.Ability.Phase2.Turn.Back")
	
	//Enemy Items
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Axe_L,"Enemy.Weapon.Axe.L")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Axe_R,"Enemy.Weapon.Axe.R")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Crossbow,"Enemy.Weapon.Crossbow")

	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Boss,"Enemy_Boss")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Target,"Enemy.Target")





	
	//ItemType

	UE_DEFINE_GAMEPLAY_TAG(Item_PrimaryWeapon,"Item.PrimaryWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_SecondaryWeapon,"Item.SecondaryWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_EquipStandbyWeapon,"Item.EquipStandbyWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_Armor,"Item.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consum,"Item.Consum");
	UE_DEFINE_GAMEPLAY_TAG(Item_PrimaryWeapon_Straightsword,"Item.PrimaryWeapon.Straightsword");
	UE_DEFINE_GAMEPLAY_TAG(Item_PrimaryWeapon_Greatsword,"Item.PrimaryWeapon.Greatsword");
	UE_DEFINE_GAMEPLAY_TAG(Item_PrimaryWeapon_Staff,"Item.PrimaryWeapon.Staff");
	UE_DEFINE_GAMEPLAY_TAG(Item_PrimaryWeapon_SacredSeal,"Item.PrimaryWeapon.SacredSeal");


	
	//Shared
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit,"Shared.Event.MeleeHit")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_RangeHit,"Shared.Event.RangeHit")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit_Parryable,"Shared.Event.MeleeHit.Parryable")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_RangeHit_Parryable,"Shared.Event.RangeHit.Parryable")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SuccessParry,"Shared.Event.SuccessParry")
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact,"Shared.Ability.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact,"Shared.Event.HitReact")

	//Block Ability Tag
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Block_Recovery_Bleed,"Shared.Ability.Block.Recovery.Bleed")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Block_Recovery_Burn,"Shared.Ability.Block.Recovery.Burn")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Block_Recovery_DeBuffAtk,"Shared.Ability.Block.Recovery.DeBuffAtk")
	
	//Block Recovery Tag
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Block_Recovery_Groggy,"Shared.SetByCaller.Block.Recovery.Groggy")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Block_Recovery_Bleed,"Shared.SetByCaller.Block.Recovery.Bleed")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Block_Recovery_Burn,"Shared.SetByCaller.Block.Recovery.Burn")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Block_Recovery_DeBuffAtk,"Shared.SetByCaller.Block.Recovery.DeBuffAtk")

	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage,"Shared.SetByCaller.BaseDamage")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Impulse,"Shared.SetByCaller.Impulse")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Burn,"Shared.SetByCaller.Burn")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Bleed,"Shared.SetByCaller.Bleed")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_DebuffAtk,"Shared.SetByCaller.DebuffAtk")

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BurnLev,"Shared.SetByCaller.BurnLev")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BleedLev,"Shared.SetByCaller.BleedLev")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_DebuffAtkLev,"Shared.SetByCaller.DebuffAtkLev")
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_TickDamage,"Shared.SetByCaller.TickDamage")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_DebuffApply,"Shared.SetByCaller.DebuffApply")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Parry,"Shared.Status.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Parried,"Shared.Status.Parried")
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front,"Shared.Status.HitReact.Front")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left,"Shared.Status.HitReact.Left")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right,"Shared.Status.HitReact.Right")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back,"Shared.Status.HitReact.Back")
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Groggy,"Shared.Ability.Groggy")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Groggy,"Shared.Status.Groggy")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death,"Shared.Ability.Death")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead,"Shared.Status.Dead")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Burn,"Shared.Status.Burn")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Burn,"Shared.Ability.Burn")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Bleed,"Shared.Status.Bleed")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Bleed,"Shared.Ability.Bleed")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Debuff_Atk,"Shared.Status.Debuff.Atk")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Debuff_Atk,"Shared.Ability.Debuff.Atk")

	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile,"Shared.Event.SpawnProjectile")
	
}