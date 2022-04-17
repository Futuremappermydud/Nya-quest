param (
    [string]$p1
 )

adb pull "/storage/emulated/0/Android/data/com.beatgames.beatsaber/files/tombstone_$p1" ./

if (Test-Path "./ndkpath.txt")
{
    $NDKPath = Get-Content ./ndkpath.txt
} else {
    $NDKPath = $ENV:ANDROID_NDK_HOME
}


$stackScript = "$NDKPath/ndk-stack"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $stackScript += ".cmd"
}

Get-Content "./tombstone_$p1" | & $stackScript -sym ./build/debug/ > "tombstone_$p1.log"
