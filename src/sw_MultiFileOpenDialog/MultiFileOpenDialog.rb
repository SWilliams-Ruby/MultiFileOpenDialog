module SW
  module MultiFileOpenDialog
    def self.entry()
      files = show_file_open_dialog()
    end
    
    
    # Open the file picker application
    # 
    #
    def self.show_file_open_dialog()
      filepaths = []
      env = {}
      prog = File.join( SW::MultiFileOpenDialog::PLUGIN_DIR, 'MultiFileOpenDialog/debug/MultiFileOpenDialog.exe' )
      cmd  = [env, prog, :err=>[:child, :out] ]

      IO.popen(cmd, mode = 'a+') { |stream|
        until stream.eof? do
          filepath = stream.readline
          filepaths << filepath
        end
      }
      puts filepaths.size
      puts filepaths
      filepaths
    end

  end
end
