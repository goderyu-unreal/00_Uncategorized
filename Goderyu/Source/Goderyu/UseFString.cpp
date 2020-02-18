// Fill out your copyright notice in the Description page of Project Settings.


#include "UseFString.h"

// Sets default values
AUseFString::AUseFString()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyString = "goderyu like ue4, and he wants to be best engineer";
	MyString.RemoveAt(1, 6);

	MyString.RemoveFromEnd("engineer");
	MyString.RemoveFromStart("like ");
	MyString.RemoveFromStart("UE4", ESearchCase::Type::CaseSensitive);

	// MyString.RemoveSpacesInline();
	/**
	 * Replace：第一个参数是待查找子串，第二个参数是欲替换子串。替换后返回值即为新的字符串，不影响原来的字符串
	 * ReplaceInline：参数含义同上，但是根据方法名可知，是直接在该字符串上操作的
	 */
	MyString = MyString.Replace(L"he", L"she");

	MyString.ReplaceCharInline(',', '@');

	MyString.ReplaceInline(L"g ", L"goderyu ");

	// reserve 储备
	// MyString.Reserve(7);

	/**
	 * Reverse和ReverseString的区别就是前者不对原字符串操作，返回值是操作后的字符串
	 * 后者直接操作原字符串
	 */
	// MyString.Reverse();
	// MyString.ReverseString();

	/**
	 * Right 返回该字符串右边几个字符
	 * RightChop 返回从该字符串头部起第几个字符的后面所有字符
	 * RightPad 填补，但是测试没发现效果
	 */
	// MyString = MyString.Right(5);
	// MyString = MyString.RightChop(2);
	// MyString = MyString.RightPad(5);
}

// Called when the game starts or when spawned
void AUseFString::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUseFString::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

