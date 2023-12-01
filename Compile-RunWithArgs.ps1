function Compile-CWithArgs {
    param(
        [Parameter(Mandatory=$false)]
        [string]$inputArgs,
        # [Parameter(Mandatory=$false)]
        # [switch]$enableDebugFlags,
        [Parameter(Mandatory=$false)]
        [string]$executableName
    )

    # Split args into array and combine them with spaces
    $inputArgs = $null
    if($inputArgs){
        $inputArgs.Split(" ") -join " "
    }

    if(!$executableName){
        $executableName = "app"
    }

    $file = $executableName + ".c"
    $executableName = ".\$executableName $inputArgs"

    # if($enableDebugFlags) {
    #     $file + " -g" # Lets debugger work with breakpoints. Not for production code!
    # }
    
    $location = Get-Location
    Set-Location $location
    gcc -o app $file -g
    Invoke-Expression $executableName
}
