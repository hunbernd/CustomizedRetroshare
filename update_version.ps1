Param(
  [string]$modname = ""
)

$env:TERM="msys";
$branch = ((git status | Select-String -Pattern branch) -split ".*branch ")[1];
$commitdata = git log -n 1 --grep="git-svn-id"
$commitid = (($commitdata | Select-String -Pattern commit) -split "commit ")[1];
(($commitdata | Select-String -Pattern git-svn-id) -match "git-svn-id.*@(?<rev>\d+) .*");
$revision = $Matches["rev"];

$htmlfile = ".\retroshare-gui\src\gui\help\version.html";
New-Item $htmlfile -type file -force;
if ($modname) {
    "Mod: $modname <br>" | Out-File $htmlfile -Encoding ASCII -Append;
}
"Git branch: $branch <br>" | Out-File $htmlfile -Encoding ASCII -Append;
"Git commit id: $commitid <br>" | Out-File $htmlfile -Encoding ASCII -Append;
"Closest svn revision: $revision <br>" | Out-File $htmlfile -Encoding ASCII -Append;
("Build date: " + (Get-Date -Format g)) | Out-File $htmlfile -Encoding ASCII -Append;

$headerfile = Get-Content ".\libretroshare\src\retroshare\rsversion_git.in";
$headerfile = $headerfile -creplace "revnum", "$revision";
$headerfile = $headerfile -creplace "modname", "$modname";
$headerfile = $headerfile -creplace "branch", "$branch";
$headerfile = $headerfile -creplace "cid", "$commitid";
$headerfile | Set-Content ".\libretroshare\src\retroshare\rsversion.h";