# Get the source and destination file paths from command line arguments
$src_file = [System.IO.Path]::GetFullPath($args[0])
$dest_file = [System.IO.Path]::GetFullPath($args[1])

# Create an Excel Application object
$oExcel = New-Object -ComObject Excel.Application

# Open the source file
$oBook = $oExcel.Workbooks.Open($src_file)

# Save the workbook as the destination file in CSV format with UTF-8 encoding (62)
$oBook.SaveAs($dest_file, 62)

# Close the workbook without saving changes
$oBook.Close($false)

# Quit Excel
$oExcel.Quit()

# Release COM objects
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($oBook) | Out-Null
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($oExcel) | Out-Null
Remove-Variable oBook
Remove-Variable oExcel
