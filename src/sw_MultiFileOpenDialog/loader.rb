require File.join(SW::MultiFileOpenDialog::PLUGIN_DIR, 'MultiFileOpenDialog')

# Menu
module SW
  module MultiFileOpenDialog
    unless @loaded
      menu = UI.menu("Plugins")
      menu.add_item("MultiFileOpenDialog") { MultiFileOpenDialog.entry() }
    end
    @loaded = true
  end
end


